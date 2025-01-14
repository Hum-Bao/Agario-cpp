import {
  drawCamera,
  drawCell,
  drawGrid,
  drawFood,
  drawCoords,
  drawVelocity,
} from "./render.js";
import { canvas, ctx, dpi } from "./canvas.js";
import { API } from "./api.js";
import { User } from "./User.js";
import { Enemy } from "./enemy.js";

class Pellet {
  X;
  Y;
  mass;
  radius;
  constructor(x, y, mass) {
    this.X = x;
    this.Y = y;
    this.mass = mass;
    this.radius = Math.sqrt(mass * 10);
  }
}

//const MAP_MIN = 0;
//const MAP_MAX = 14142;
const MAP_WIDTH = 4000;
const MAP_HEIGHT = 4000;

//Active camera movement position
let mouseX = MAP_WIDTH / 2;
let mouseY = MAP_HEIGHT / 2;

const enemies = new Map();
//var pellets = new Map();
const pellets = new Set();

let frameCount = 0;

let tickrate = 1000 / 60;
const massloserate = 0.02;
//var pellets = [];

API.onMessage("PELLET", (message, [x, y, mass]) => {
  pellets.add(new Pellet(parseInt(x), parseInt(y), parseInt(mass)));
  // pellets.set(parseInt(x), parseInt(y));
});

API.onMessage("ENEMY", (message, [id, x, y, mass, radius]) => {
  enemies.set(id, new Enemy(x, y, mass, radius));
  /*
  console.log(
    "ENEMY: " +
      id +
      " AT: " +
      x +
      "," +
      y +
      " MASS: " +
      mass +
      " RADIUS: " +
      radius
  );
  */
});

API.onMessage("CONSUMED", (message, [x, y]) => {
  for (const Pellet of pellets) {
    if (Pellet.X == x && Pellet.Y == y) {
      pellets.delete(Pellet);
    }
  }
  pellets.delete(parseInt(x));
  console.log("DELETING PELLET EATEN AT: " + x + "," + y);
});

API.onMessage("GAMEOVER", (message) => {
  window.location.reload();
});

/*
API.onMessage("DESYNC", (message, [x, y]) => {
  User.X = parseFloat(x);
  User.Y = parseFloat(y);
  console.log("DESYNC:" + x + "," + y);
});
*/

const GetPos = (x, y) => {
  return {
    x: x - User.X + canvas.width / 2,
    y: y - User.Y + canvas.height / 2,
  };
};

function render() {
  // Clear the canvas
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  drawGrid(User.X, User.Y);

  //for (const [x, y] of pellets) {
  for (const Pellet of pellets) {
    let pos = GetPos(Pellet.X, Pellet.Y);
    drawFood(pos.x, pos.y, Pellet.radius);
  }

  for (const enemy of enemies.values()) {
    let pos = GetPos(enemy.X, enemy.Y);
    drawCell(pos.x, pos.y, enemy.radius);
  }

  drawCamera(User.X, User.Y);
  // drawCoords(User.X, User.Y);
  //drawVelocity(User.current_velocity.X, User.current_velocity.Y);
}

function updateVelocity() {
  let velocityX = 0;
  let velocityY = 0;
  // Move the camera toward the mouse position at a fixed speed
  const dx = mouseX - (User.X + canvas.width / 2);
  const dy = mouseY - (User.Y + canvas.height / 2);
  const distance = Math.hypot(dx, dy);

  if (distance < 20) {
    velocityX = 0;
    velocityY = 0;
  } else {
    velocityX = (dx / distance) * User.speed;
    velocityY = (dy / distance) * User.speed;
  }

  if (User.last_velocity != User.current_velocity) {
    User.last_velocity = User.current_velocity;
    User.current_velocity = { X: velocityX, Y: velocityY };
  }
}

window.addEventListener("resize", resize);

function resize() {
  canvas.width = window.innerWidth * dpi;
  canvas.height = window.innerHeight * dpi;
  User.updateViewport(canvas.width, canvas.height);
}

window.addEventListener("beforeunload", function () {
  API.send("KILLED:self");
  return false;
});

window.requestAnimationFrame = (function () {
  return (
    window.requestAnimationFrame ||
    window.webkitRequestAnimationFrame ||
    window.mozRequestAnimationFrame ||
    window.msRequestAnimationFrame ||
    function (callback) {
      window.setTimeout(callback, tickrate);
    }
  );
})();

window.cancelAnimationFrame = (function () {
  return window.cancelAnimationFrame || window.mozCancelAnimationFrame;
})();

function clamp() {
  User.X = Math.max(0, Math.min(User.X, MAP_WIDTH));
  User.Y = Math.max(0, Math.min(User.Y, MAP_HEIGHT));
}

function checkPlayerCollision() {
  for (const [id, Enemy] of enemies) {
    const dx = User.X - Enemy.X;
    const dy = User.Y - Enemy.Y;
    const distance = Math.hypot(dx, dy);
    if (
      Enemy.radius < User.radius * 0.85 &&
      distance + Enemy.radius <= User.radius
    ) {
      API.send("KILLED:" + id);
      console.log("KILLED:" + id);
      User.eat(Enemy.mass * 0.72);
      enemies.delete(id);
    }
  }
}

function checkPelletCollision() {
  //for (const [x, y] of pellets) {
  for (const Pellet of pellets) {
    //const dx = User.X - x;
    //const dy = User.Y - y;
    const dx = User.X - Pellet.X;
    const dy = User.Y - Pellet.Y;
    const distance = Math.hypot(dx, dy);

    // Check if the distance is less than the sum of the player's radius and the pellet's radius
    if (distance < User.radius) {
      // Collision detected, remove the pellet from the map
      //pellets.delete(x);
      //API.send("CONSUMED:" + x + "," + y);
      API.send("CONSUMED:" + Pellet.X + "," + Pellet.Y);
      User.eat(Pellet.mass);
      pellets.delete(Pellet);
      return true;
    }
  }
  return false;
}

function gameLoop() {
  frameCount++;
  User.X += User.current_velocity.X;
  User.Y += User.current_velocity.Y;
  /*
  if (
    User.last_velocity.X != User.current_velocity.X &&
    User.last_velocity.Y != User.current_velocity.Y
  ) {
    API.send(
      "VELOCITY:" + User.current_velocity.X + "," + User.current_velocity.Y
    );
  }
  */

  // Clamp the camera within the game world
  clamp();

  if (frameCount % 60 == 0) {
    User.eat(-1 * (User.mass * 0.0025));
  }
  /*
  if (frameCount % 5 === 0) {
    API.send("LOCATION:" + User.X + "," + User.Y);
  }
    */

  checkPelletCollision();
  API.send("LOCATION:" + User.X + "," + User.Y);
  checkPlayerCollision();
  //console.log(cameraX + "," + cameraY);

  render();
  window.requestAnimationFrame(gameLoop);
}

function startGame() {
  const serverip = prompt(
    "Please enter the server ip and port (E.g. 127.0.0.1:8080)"
  );

  API.server = serverip;
  API.initialize();
  //user = new User();
  User.initialize().then(() => {
    console.log("Starting");
    gameLoop();
  });
}

window.onload = function () {
  canvas.addEventListener("mousemove", (e) => {
    const rect = canvas.getBoundingClientRect();
    mouseX = User.X + (e.clientX - rect.left);
    mouseY = User.Y + (e.clientY - rect.top);

    updateVelocity();
  });
  startGame();
};
