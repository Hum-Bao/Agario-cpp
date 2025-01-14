import { canvas, ctx } from "./canvas.js";
import { User } from "./User.js";
//server size is from -7071:-7071 to 7071:7071 or 14142x14142 units
//const MAP_MIN = -7071;
//const MAP_MAX = 7071;
const MAP_WIDTH = 4000;
const MAP_HEIGHT = 4000;
const GRID_SIZE = 50;

//For generating food, create drawFood function.
//Have either client receive all food and draw only that within viewport, or have server only send food near player.
//Have array to hold food with positions?

export function drawGrid(cameraX, cameraY) {
  // Draw the grid background

  ctx.fillStyle = "white";
  ctx.fillRect(0, 0, canvas.width, canvas.height);
  ctx.strokeStyle = "lightgray";
  ctx.lineWidth = 1;
  // ctx.globalAlpha = 0.15;

  const startX = Math.floor(cameraX / GRID_SIZE) * GRID_SIZE;
  const startY = Math.floor(cameraY / GRID_SIZE) * GRID_SIZE;
  ctx.beginPath();

  for (let x = startX; x <= cameraX + canvas.width; x += GRID_SIZE) {
    ctx.moveTo(x - cameraX, 0);
    ctx.lineTo(x - cameraX, canvas.height);
  }

  for (let y = startY; y <= cameraY + canvas.height; y += GRID_SIZE) {
    ctx.moveTo(0, y - cameraY);
    ctx.lineTo(canvas.width, y - cameraY);
  }
  ctx.stroke();
}

// Helper to check if the circle is at the edge of the camera view
function isCircleAtEdge(cameraX, cameraY) {
  return {
    top: cameraY <= 0,
    bottom: cameraY >= MAP_HEIGHT,
    // bottom: cameraY >= MAP_HEIGHT,
    left: cameraX <= 0,
    right: cameraX >= MAP_WIDTH,
    // right: cameraX >= MAP_WIDTH,
    // right: cameraX >= MAP_WIDTH,
  };
}

export function drawFood(x, y, radius) {
  ctx.beginPath();
  ctx.arc(x, y, radius, 0, Math.PI * 2);
  ctx.fillStyle = "red";
  ctx.fill();
  ctx.closePath();
}

export function drawCamera(cameraX, cameraY) {
  // Circle (player) properties
  // const circleRadius = 20;
  const circleX = canvas.width / 2; // Fixed on the canvas
  const circleY = canvas.height / 2;

  const edges = isCircleAtEdge(cameraX, cameraY);
  ctx.beginPath();

  if (edges.top) {
    // Top edge: Clip the top portion
    ctx.arc(circleX, circleY, User.radius, 0, Math.PI, false); // Bottom half
  } else if (edges.bottom) {
    // Bottom edge: Clip the bottom portion
    ctx.arc(circleX, circleY, User.radius, 0, Math.PI, true); // Top half
  } else if (edges.left) {
    // Left edge: Clip the left portion
    ctx.arc(circleX, circleY, User.radius, -Math.PI / 2, Math.PI / 2, false); // Right half
  } else if (edges.right) {
    // Right edge: Clip the right portion
    ctx.arc(circleX, circleY, User.radius, Math.PI / 2, -Math.PI / 2, false); // Left half
  } else {
    // Default circle
    ctx.arc(circleX, circleY, User.radius, 0, Math.PI * 2);
  }

  ctx.fillStyle = "blue";
  ctx.fill();
  ctx.closePath();
}

export function drawCell(x, y, radius) {
  // Circle (Enemy) properties

  const edges = isCircleAtEdge(x, y);
  ctx.beginPath();

  if (edges.top) {
    // Top edge: Clip the top portion
    ctx.arc(x, y, radius, 0, Math.PI, false); // Bottom half
  } else if (edges.bottom) {
    // Bottom edge: Clip the bottom portion
    ctx.arc(x, y, radius, 0, Math.PI, true); // Top half
  } else if (edges.left) {
    // Left edge: Clip the left portion
    ctx.arc(x, y, radius, -Math.PI / 2, Math.PI / 2, false); // Right half
  } else if (edges.right) {
    // Right edge: Clip the right portion
    ctx.arc(x, y, radius, Math.PI / 2, -Math.PI / 2, false); // Left half
  } else {
    // Default circle
    ctx.arc(x, y, radius, 0, Math.PI * 2);
  }

  ctx.fillStyle = "green";
  ctx.fill();
  ctx.closePath();
}

export function drawCoords(cameraX, cameraY) {
  ctx.fillStyle = "black";
  ctx.font = "bold 16px Calibri";
  ctx.textAlign = "left";
  ctx.textBaseline = "bottom";
  ctx.fillText(cameraX + ", " + cameraY, 0, canvas.height);
}

export function drawVelocity(velocityX, velocityY) {
  ctx.fillStyle = "black";
  ctx.font = "bold 16px Calibri";
  ctx.textAlign = "left";
  //ctx.textBaseline = "bottom";
  ctx.fillText(velocityX + ", " + velocityY, 0, canvas.height / 2);
}
