import { API } from "./api.js";
export class User {
  //api = new API();
  static X = 0;
  static Y = 0;
  viewport_x = 1920;
  viewport_y = 1080;
  static spawned = false;
  static mass = 10;
  //static radius = Math.sqrt(User.mass * 100);
  static radius = Math.sqrt(User.mass * 40);
  // static speed = 2.2 * Math.pow(User.mass, -0.439);
  static speed = 4.4 * Math.pow(User.mass, -0.439);

  static last_velocity = { X: parseInt(0), Y: parseInt(0) };
  static current_velocity = { X: parseInt(0), Y: parseInt(0) };

  /*
  constructor() {
    User.initialize();
  }
    */

  static eat(size) {
    User.mass += size;
    if (User.mass < 10) {
      User.mass = 10;
    }
    // User.radius = Math.sqrt(User.mass * 100);
    User.radius = Math.sqrt(User.mass * 40);
    //User.speed = 2.2 * Math.pow(User.mass, -0.439);
    User.speed = 4.4 * Math.pow(User.mass, -0.439);
    API.send("SIZE:" + User.mass + "," + User.radius);
  }

  static async initialize() {
    await API.waitUntilReady(); // Wait for WebSocket to be ready
    // this.viewport_x = 1920;
    // this.viewport_y = 1080;
    // API.send("SPAWN & VIEWPORT:" + this.viewport_x + "," + this.viewport_y);
    API.send("SPAWN");
    const spawnData = await API.waitForSpawn();
    User.X = parseInt(spawnData.x);
    User.Y = parseInt(spawnData.y);
    User.mass = parseInt(spawnData.mass);
    User.radius = parseInt(spawnData.radius);

    // this.updateViewport(window.innerWidth, window.innerHeight);

    /*
    this.api.onMessage("SPAWNED", (message, [x, y]) =>
      this.handleSpawnMessage(x, y)
    );
    */
    //this.api.send("SPAWN");
    //console.log("SENDING: " + x + "," + y);
  }

  static updateViewport(viewport_x, viewport_y) {
    API.send("VIEWPORT:" + viewport_x + "," + viewport_y);
  }

  static move() {
    // User.X = new_x;
    // User.Y = new_y;
    API.send("AT:" + User.X + "," + User.Y);
    // console.log(cameraX + "," + cameraY);
  }
}
