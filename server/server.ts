const path = require("path");
const express = require("express");
const socketIO = require("socket.io");
const cors = require("cors")

const PORT = process.env.PORT || 3000;

const app = express();
const buildPath = path.join(__dirname, '..', 'build');
app.use(express.static(buildPath));
app.use(cors());

app.post("/event", (req, res) => {
    io.emit(req.body);
});

const io = socketIO(app);

io.on("connection", (socket) => {
    console.log("Client connected");
    socket.on("disconnect", () => console.log("Client disconnected"));
});

