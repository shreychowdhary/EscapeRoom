import React from 'react';
import io from 'socket.io-client';
interface LocationPuzzleSolution {
    index: number;
    solution: string;
}

function App() {
    const socketIO = React.useRef<SocketIOClient.Socket>(io());

    React.useEffect(() => {
        socketIO.current.on('Translate', () => {
            console.log('Translate');
        });
        socketIO.current.on('Location', (data: LocationPuzzleSolution) => {
            console.log(data.index);
        });
        socketIO.current.on('End', () => {
            console.log('End');
        });
    }, []);

    return <div className="App"></div>;
}

export default App;
