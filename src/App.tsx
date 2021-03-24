import React from 'react';
import io from "socket.io-client";
import './App.css';

function App() {
    const socketIO = React.useRef<SocketIOClient.Socket>(io("localhost:3000"));

    return (
        <div className="App">

        </div>
    );
}

export default App;
