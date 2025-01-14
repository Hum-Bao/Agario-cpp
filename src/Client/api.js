export class API {
  static socket;
  static ready = false;
  static players;
  static pellets;
  static messageHandlers = new Map();
  static server;

  static initialize() {
    API.ready = false;
    API.players = new Map();
    //API.messageHandlers = new Map();
    if (!API.socket) {
      API.socket = new WebSocket("ws://" + API.server);

      API.socket.onopen = () => {
        console.log("WebSocket open\n");
        API.ready = true;
      };

      API.socket.onmessage = (event) => {
        API.handleMessage(event.data);
      };
    }

    API.socket.onclose = () => {
      console.log("WebSocket Closed\n");
      API.socket.send("DEATH");
    };
  }

  static send(message) {
    if (API.socket.readyState === WebSocket.OPEN) {
      API.socket.send(message);
    } else {
      console.error(
        "WebSocket is not open. Ready state: " + API.socket.readyState
      );
    }
  }

  static waitUntilReady() {
    return new Promise((resolve) => {
      API.socket.onopen = resolve;
    });
  }

  static waitForSpawn() {
    return new Promise((resolve, reject) => {
      // Listen for the SPAWN message and resolve the promise
      this.onMessage("SPAWNED", (message, [x, y, mass, radius]) => {
        console.log("SPAWN message received:", x, y, mass, radius);
        resolve({ x, y, mass, radius }); // Resolve with spawn position
      });
    });
  }

  static handleMessage(message) {
    const [type, ...data] = message.split(":");
    if (API.messageHandlers.has(type)) {
      API.messageHandlers.get(type)(message, data);
    }
  }

  static onMessage(type, callback) {
    API.messageHandlers.set(type, callback);
  }
}
