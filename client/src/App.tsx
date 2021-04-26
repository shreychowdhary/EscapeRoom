import React from 'react';
import io from 'socket.io-client';
import SplitPane from 'react-split-pane';
import { List, ListItem, Typography, Divider, Card, Button, CardContent } from '@material-ui/core';
import { makeStyles } from '@material-ui/core/styles';
import useSound from 'use-sound';

interface LocationPuzzleSolution {
    index: number;
    planetId: string;
    solution: string;
}

const useStyles = makeStyles((theme) => ({
    list: {
        backgroundColor: theme.palette.background.paper,
    },
    communicationTitle: {
        textAlign: 'center',
    },
    solar: {
        width: '100%',
        height: '100%',
        display: 'flex',
    },
    card: {
        width: '100%',
    },
}));

function App() {
    const classes = useStyles();

    const TranslatedMessage = () => (
        <ListItem>
            <Card className={classes.card} variant="outlined">
                <CardContent>
                    <Typography>
                        Message: Your species has kidnapped our child. We take this as an act of war. Tell us where they
                        are or else; we will send the armada for retrieval. The clock is ticking.
                    </Typography>
                    <Typography color="textSecondary">From: Baby Bigfoot&apos;s Parents</Typography>
                </CardContent>
            </Card>
        </ListItem>
    );

    const SolarSystemMessage = () => (
        <ListItem className="SolarSystemMessage">
            <Card className={classes.card} variant="outlined">
                <CardContent>
                    <Typography>Solar System Received</Typography>
                    <Typography color="textSecondary">From: Location Tablet</Typography>
                </CardContent>
            </Card>
        </ListItem>
    );

    const EndMessage = () => (
        <ListItem className="EndMessage">
            <Card className={classes.card} variant="outlined">
                <CardContent>
                    <Typography>
                        We have received your transmission and would like to send out a thanks for returning our child.
                        Because of this, we will simply return to our home planet without destroying yours.
                    </Typography>
                    <Typography color="textSecondary">From: Baby Bigfoot&apos;s Parents</Typography>
                </CardContent>
            </Card>
        </ListItem>
    );

    const socketIO = React.useRef<SocketIOClient.Socket>(io());
    const canvasRef = React.useRef<HTMLCanvasElement>(null);

    const [showTranslatedMessage, setShowTranslatedMessage] = React.useState(false);
    const [showSolarSystem, setShowSolarSystem] = React.useState(false);
    const [showEndMessage, setShowEndMessage] = React.useState(false);

    const [playDrAudio] = useSound('./sounds/dr_audio_file.mp3');

    React.useEffect(() => {
        socketIO.current.on('Translate', () => {
            setShowTranslatedMessage(true);
        });
    }, [showTranslatedMessage]);

    React.useEffect(() => {
        socketIO.current.on('Location', (data: LocationPuzzleSolution) => {
            if (canvasRef.current) {
                const canvas = canvasRef.current;
                const context = canvas.getContext('2d');
                if (context) {
                    if (data.index == 1) {
                        setShowSolarSystem(true);
                        for (let i = 0; i < 5; i++) {
                            context.beginPath();
                            context.arc(400, 400, 70 * (i + 1), 0, 2 * Math.PI);
                            context.stroke();
                        }
                        context.beginPath();
                        context.arc(400, 400, 20, 0, 2 * Math.PI);
                        context.fillStyle = 'yellow';
                        context.stroke();
                        context.fill();
                    }
                    const image = new Image();
                    image.src = `images/Planet${data.index}.png`;
                    image.onload = () => {
                        context.drawImage(image, 400 - data.index * 70 - 25, 400 - 25, 50, 50);
                    };
                    if (data.index == 5) {
                        context.fillStyle = 'blue';
                        context.font = '30px Arial';
                        const labels = ['21', '33', '5', '55', '8'];
                        for (let i = 0; i < 5; i++) {
                            context.fillText(labels[i], 400 - i * 70 - 90, 400 - 30);
                        }
                    }
                }
            }
        });
        socketIO.current.on('End', () => {
            setShowEndMessage(true);
        });
    }, []);

    return (
        <div className="App">
            <SplitPane split="vertical">
                <div className="Translator">
                    <Typography variant="h2" className={classes.communicationTitle}>
                        Communications
                    </Typography>
                    <Divider></Divider>
                    <List className={classes.list}>
                        <ListItem>
                            <Card className={classes.card} variant="outlined">
                                <CardContent>
                                    <Typography>Message Received:</Typography>
                                    <Typography>
                                        Um is this thing on? Uh well hi my name is Dr. Beeup and if you&apos;re hearing
                                        this that means you were able to open up the box. Congratulations. Um however,
                                        there is some bad news. In this big blue box, do you see that cage? Well um
                                        that&apos;s what we call Baby Bigfoot here on Earth but it&apos;s actually an
                                        alien life form that NASA was experimenting on and his family was not too happy
                                        about that, and they&apos;re headed here right now. In the black box are some
                                        devices to figure out their location and communicate with his family. Good luck.
                                        I think the admins are starting to get suspicious of me, so I have to get going.
                                        Bye!
                                    </Typography>
                                    <Button
                                        onClick={() => {
                                            playDrAudio();
                                        }}
                                    >
                                        Play Audio
                                    </Button>
                                    <Typography color="textSecondary">From: Dr.Z</Typography>
                                </CardContent>
                            </Card>
                        </ListItem>
                        <ListItem className="UntranslatedMessage">
                            <Card className={classes.card} variant="outlined">
                                <CardContent>
                                    <Typography>Message Received: Use Translator to translate</Typography>
                                    <img src="images/UntranslatedMessage.png" />
                                    <Typography color="textSecondary">From: Dr.BF</Typography>
                                </CardContent>
                            </Card>
                        </ListItem>
                        {showTranslatedMessage ? <TranslatedMessage /> : null}
                        {showSolarSystem ? <SolarSystemMessage /> : null}
                        {showEndMessage ? <EndMessage /> : null}
                    </List>
                </div>
                <div className={classes.solar}>
                    <canvas ref={canvasRef} width="800" height="800" />
                </div>
            </SplitPane>
        </div>
    );
}

export default App;
