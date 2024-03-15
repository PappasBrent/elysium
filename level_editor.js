function roundToNearest(val, rounder) {
    return Math.round(val / rounder) * rounder;
}

window.onload = function () {
    const
        /** @type{HTMLTextAreaElement} */
        output = document.getElementById('output'),
        /** @type{HTMLButtonElement} */
        exportBtn = document.getElementById('exportBtn'),
        /** @type{HTMLButtonElement} */
        importBtn = document.getElementById('importBtn'),
        /** @type{HTMLCanvasElement} */
        canvas = document.getElementById("gameCanvas"),
        ctx = canvas.getContext('2d'),
        CELL_SIZE = 32,
        SCENE_WIDTH_COLS = 48,
        SCENE_HEIGHT_ROWS = 16,
        W = SCENE_WIDTH_COLS * CELL_SIZE,
        H = SCENE_HEIGHT_ROWS * CELL_SIZE,
        grid = [];
    let
        mouseX, mouseY,
        mouseGridX, mouseGridY,
        mouseGridC = 0, mouseGridR = 0,
        char = ',',
        drawing = false,
        erasing = false;

    canvas.setAttribute('width', W);
    canvas.setAttribute('height', H);
    ctx.font = '24px Mono';


    for (let i = 0; i < SCENE_HEIGHT_ROWS; i++) {
        row = new Array(SCENE_WIDTH_COLS).fill(',');
        grid.push(row);
    }

    exportBtn.onclick = function () {
        let s = '';
        for (let r = 0; r < SCENE_HEIGHT_ROWS; r++) {
            for (c = 0; c < SCENE_WIDTH_COLS; c++) {
                s += grid[r][c];
            }
            if (r !== SCENE_HEIGHT_ROWS) {
                s += '\n';
            }
        }
        output.value = s;
    }

    importBtn.onclick = function () {
        const s = output.value;
        if (s.length > 0) {
            let r = 0, c = 0;
            for (let i = 0; i < s.length; i++) {
                if (s[i] === '\n') {
                    r++;
                    c = 0;
                } else {
                    grid[r][c] = s[i];
                    c++;
                }
            }
        }
    }

    canvas.addEventListener('mousemove', function onMouseMove(event) {
        mouseX = event.clientX - canvas.offsetLeft;
        mouseY = event.clientY - canvas.offsetTop;
        mouseGridX = roundToNearest(mouseX - CELL_SIZE / 2, CELL_SIZE);
        mouseGridY = roundToNearest(mouseY - CELL_SIZE / 2, CELL_SIZE);
        mouseGridR = mouseGridY / CELL_SIZE;
        mouseGridC = mouseGridX / CELL_SIZE;
    });

    window.addEventListener('contextmenu', event => event.preventDefault());

    canvas.addEventListener('mousedown', function onMouseDown(event) {
        if (event.button == 0) {
            drawing = true;
        } else if (event.button == 2) {
            erasing = true;
        } else if (event.button == 1) {
            floodFill(mouseGridR, mouseGridC);
        }
    });

    canvas.addEventListener('mouseup', function onMouseUp(event) {
        drawing = false;
        erasing = false;
    });

    document.addEventListener('keypress', function onKeyPress(event) {
        if (event.key !== 'Enter') {
            char = event.key;
        }
    });

    function floodFill(startR, startC) {
        const
            startChar = grid[startR][startC],
            q = [[startR, startC]];
        grid[startR][startC] = char;
        while (q.length > 0) {
            const [r, c] = q.shift();

            if (0 <= r - 1 && grid[r - 1][c] == startChar && grid[r - 1][c] != char) {
                grid[r - 1][c] = char;
                q.push([r - 1, c]);
            }
            if (0 <= c - 1 && grid[r][c - 1] == startChar && grid[r][c - 1] != char) {
                grid[r][c - 1] = char;
                q.push([r, c - 1]);
            }
            if (r + 1 < SCENE_HEIGHT_ROWS && grid[r + 1][c] == startChar && grid[r + 1][c] != char) {
                grid[r + 1][c] = char;
                q.push([r + 1, c]);
            }
            if (c + 1 < SCENE_WIDTH_COLS && grid[r][c + 1] == startChar && grid[r][c + 1] != char) {
                grid[r][c + 1] = char;
                q.push([r, c + 1]);
            }
        }
    }

    function draw() {
        ctx.clearRect(0, 0, W, H);

        ctx.strokeStyle = 'black';
        ctx.lineWidth = 1;

        ctx.strokeRect(0, 0, W, H);
        for (let y = 0; y < H; y += CELL_SIZE) {
            for (let x = 0; x < W; x += CELL_SIZE) {
                const r = y / CELL_SIZE, c = x / CELL_SIZE;
                ctx.fillStyle = 'black';
                ctx.fillRect(x, y, CELL_SIZE, CELL_SIZE);
                ctx.strokeStyle = 'white';
                ctx.strokeRect(x, y, CELL_SIZE, CELL_SIZE);
                switch (grid[r][c]) {
                    case ',':
                        ctx.fillStyle = 'green';
                        break;

                    case '~':
                        ctx.fillStyle = 'cyan';
                        break;

                    case '-':
                        ctx.fillStyle = 'blue';
                        break;

                    case '^':
                        ctx.fillStyle = 'yellow';
                        break;

                    case '+':
                        ctx.fillText('🌸', x, y + CELL_SIZE - 2);
                        continue;

                    default:
                        ctx.fillStyle = 'white';
                        break;
                }
                ctx.fillText(grid[r][c], x, y + CELL_SIZE - 2);
            }
        }

        ctx.strokeStyle = 'orange';
        ctx.lineWidth = 5;

        ctx.strokeRect(mouseGridX, mouseGridY, CELL_SIZE, CELL_SIZE);
    }

    function update() {
        if (drawing) {
            grid[mouseGridR][mouseGridC] = char;
        }
        if (erasing) {
            grid[mouseGridR][mouseGridC] = '';
        }
    }

    function gameLoop() {
        update();
        draw();
        window.requestAnimationFrame(gameLoop);
    }

    draw();
    gameLoop();
}