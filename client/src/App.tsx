import React from 'react';
import io from 'socket.io-client';

function App() {
    const socketIO = React.useRef<SocketIOClient.Socket>(io('localhost:8080'));

    React.useEffect(() => {
        socketIO.current.on('event', (data: string) => console.log(data));
    }, []);

    return <div className="App"></div>;
}

export default App;
