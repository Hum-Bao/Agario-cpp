//import { API } from "./api.js";
export class Enemy {
  X;
  Y;
  radius;
  mass;

  constructor(X, Y, mass, radius) {
    this.X = parseInt(X);
    this.Y = parseInt(Y);
    this.mass = parseInt(mass);
    this.radius = parseInt(radius);
  }

  GetX() {
    return this.X;
  }

  GetY() {
    return this.Y;
  }

  GetMass() {
    return this.mass;
  }
}
