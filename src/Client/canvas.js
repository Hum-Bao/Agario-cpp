const canvas = document.getElementById("gameCanvas");
const ctx = canvas.getContext("2d", { alpha: false });
const dpi = window.devicePixelRatio || 1;
canvas.width = window.innerWidth * dpi;
canvas.height = window.innerHeight * dpi;
export { canvas, ctx, dpi };
