import React from 'react';
import io from 'socket.io-client';
import SplitterLayout from 'react-splitter-layout';
import './App.css';
interface LocationPuzzleSolution {
    index: number;
    planetId: string;
    solution: string;
}

function App() {
    const socketIO = React.useRef<SocketIOClient.Socket>(io());
    const canvasRef = React.useRef<HTMLCanvasElement>(null);

    React.useEffect(() => {
        socketIO.current.on('Translate', () => {
            console.log('Translate');
        });
        socketIO.current.on('Location', (data: LocationPuzzleSolution) => {
            if (canvasRef.current) {
                const canvas = canvasRef.current;
                const context = canvas.getContext('2d');
                if (context) {
                    context.beginPath();
                    context.fillStyle = 'blue';
                    context.arc(298 - data.index * 70, 400, 20, 0, 2 * Math.PI);
                    context.fill();
                    context.font = 'bold 20px Arial';
                    context.fillStyle = 'black';
                    context.fillText(data.planetId, 310 - data.index * 70, 380);
                }
            }
        });
        socketIO.current.on('End', () => {
            console.log('End');
        });
    }, []);

    return (
        <div className="App">
            <SplitterLayout>
                <div className="Translator">
                    <label>
                        Message:
                        <input type="text" name="name" />
                    </label>
                    <input type="submit" value="Submit" />
                </div>
                <div className="Solar">
                    <canvas className="SolarCanvas" ref={canvasRef} width="800" height="800" />
                </div>
            </SplitterLayout>
        </div>
    );
}

export default App;
