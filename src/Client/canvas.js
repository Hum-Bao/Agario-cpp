/*
class Canvas {
    constructor() {
        this.canvas = document.getElementById('gameCanvas');
        this.canvas.width = window.innerWidth;
        this.canvas.height = window.innerHeight;
        var mouseX = 0;
        var mouseY = 0;
        this.canvas.addEventListener('mousemove', (e) => {
            const rect = canvas.getBoundingClientRect();
            mouseX = cameraX + (e.clientX - rect.left);
            mouseY = cameraY + (e.clientY - rect.top)
        });


    }
}*/

const canvas = document.getElementById("gameCanvas");
const ctx = canvas.getContext("2d", { alpha: false });
//ctx.imageSmoothingEnabled = "false";
//const ctx = canvas.getContext("2d");
const dpi = window.devicePixelRatio || 1;
canvas.width = window.innerWidth * dpi;
canvas.height = window.innerHeight * dpi;
//canvas.style.position = "absolute";
//canvas.style.left = `${(window.innerWidth - canvas.width) / 2}px`;
//canvas.style.top = `${(window.innerHeight - canvas.height) / 2}px`;
export { canvas, ctx, dpi };

/*
    <script>
        // Canvas setup
        const canvas = document.getElementById('gameCanvas');
        const ctx = canvas.getContext('2d');
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;

        // Game world dimensions
        const worldWidth = 14142;
        const worldHeight = 14142;

        // Circle (player) properties
        const circleRadius = 20;
        const circleX = canvas.width / 2; // Fixed on the canvas
        const circleY = canvas.height / 2;

        // Viewport (camera) position
        let cameraX = 0;
        let cameraY = 0;

        // Mouse position in the game world
        let targetMouseX = worldWidth / 2;
        let targetMouseY = worldHeight / 2;

        // Active camera movement position
        let mouseX = targetMouseX;
        let mouseY = targetMouseY;

        // Background grid properties
        const gridSize = 50;

        // Speed of camera movement
        const cameraSpeed = 1.5;

        // Delay before switching directions
        const directionChangeDelay = 180; // in milliseconds (0.01 seconds)
        let directionChangeTimer = null;

        // Update mouse position
        canvas.addEventListener('mousemove', (e) => {
            const rect = canvas.getBoundingClientRect();
            mouseX = cameraX + (e.clientX - rect.left);
            mouseY = cameraY + (e.clientY - rect.top);
        });

        // Helper to check if the circle is at the edge of the camera view
        function isCircleAtEdge() {
            return {
                top: cameraY <= 0,
                bottom: cameraY + canvas.height >= worldHeight,
                left: cameraX <= 0,
                right: cameraX + canvas.width >= worldWidth
            };
        }

        // Draw the circle with smashing effect
        function drawCircleWithEdgeEffect() {
            const edges = isCircleAtEdge();
            ctx.beginPath();

            if (edges.top) {
                // Top edge: Clip the top portion
                ctx.arc(circleX, circleY, circleRadius, 0, Math.PI, false); // Bottom half
            } else if (edges.bottom) {
                // Bottom edge: Clip the bottom portion
                ctx.arc(circleX, circleY, circleRadius, 0, Math.PI, true); // Top half
            } else if (edges.left) {
                // Left edge: Clip the left portion
                ctx.arc(circleX, circleY, circleRadius, -Math.PI / 2, Math.PI / 2, false); // Right half
            } else if (edges.right) {
                // Right edge: Clip the right portion
                ctx.arc(circleX, circleY, circleRadius, Math.PI / 2, -Math.PI / 2, false); // Left half
            } else {
                // Default circle
                ctx.arc(circleX, circleY, circleRadius, 0, Math.PI * 2);
            }

            ctx.fillStyle = 'blue';
            ctx.fill();
            ctx.closePath();
        }


        // Game loop
        function gameLoop() {
            // Move the camera toward the delayed mouse position at a fixed speed
            const dx = mouseX - (cameraX + circleX);
            const dy = mouseY - (cameraY + circleY);
            const distance = Math.sqrt(dx * dx + dy * dy);

            if (distance > 1) {
                cameraX += (dx / distance) * cameraSpeed;
                cameraY += (dy / distance) * cameraSpeed;
            }

            // Clamp the camera within the game world
            cameraX = Math.max(0, Math.min(cameraX, worldWidth - canvas.width));
            cameraY = Math.max(0, Math.min(cameraY, worldHeight - canvas.height));

            // Clear the canvas
            ctx.clearRect(0, 0, canvas.width, canvas.height);

            // Draw the grid background
            ctx.strokeStyle = 'lightgray';
            ctx.lineWidth = 1;

            const startX = Math.floor(cameraX / gridSize) * gridSize;
            const startY = Math.floor(cameraY / gridSize) * gridSize;

            for (let x = startX; x <= cameraX + canvas.width; x += gridSize) {
                ctx.beginPath();
                ctx.moveTo(x - cameraX, 0);
                ctx.lineTo(x - cameraX, canvas.height);
                ctx.stroke();
            }

            for (let y = startY; y <= cameraY + canvas.height; y += gridSize) {
                ctx.beginPath();
                ctx.moveTo(0, y - cameraY);
                ctx.lineTo(canvas.width, y - cameraY);
                ctx.stroke();
            }
            /*
            // Draw the circle at the center of the canvas
            ctx.beginPath();
            ctx.arc(circleX, circleY, circleRadius, 0, Math.PI * 2);
            ctx.fillStyle = 'blue';
            ctx.fill();
            ctx.closePath();
            drawCircleWithEdgeEffect();

            // Loop the game
            requestAnimationFrame(gameLoop);
        }

        // Start the game loop
        gameLoop();
    </script>

</html>
*/
