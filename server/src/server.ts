import express from "express";
import bodyParser from "body-parser";
import path from "path";
import { Server } from "socket.io";
import cors from "cors";

const PORT = process.env.PORT || 8080;

const app = express();
const clientBuildPath = path.join(__dirname, '../../client/build');
app.use(express.static(clientBuildPath));
app.use(cors());
app.use(bodyParser.json());


const server = app.listen(PORT, () => console.log(`Listening on ${PORT}`));
const io = new Server(server, {
    cors: {
        origin: "*",
    }
});

app.post("/event", (req, res) => {
    io.emit(req.body.name, req.body.data);
    res.send("Successful");
});


io.on("connection", (socket) => {
    console.log("Client connected");
    socket.on("disconnect", () => console.log("Client disconnected"));
});

export default app;