import './styles.css'
import Button from '@material-ui/core/Button';
import { useAuth } from '../../context/AuthContext';
import { useEffect } from 'react';
import { useServer } from '../../context/ServerContext';


export default function StockCard(props) {
    const {token} = useAuth();
    const {ws, connected} = useServer();

    const handleBuy = () => {
        let request = {
            "fun": "buystock",
            "data": {
                "email": token,
                "company": props.name
            }
        }
        console.log('request: ', request);
        if (ws && connected)
            ws.send(JSON.stringify(request));
    }

    useEffect(() => {
        if (ws && connected){
            ws.onmessage = evt => {
                if (evt.data){
                    evt = JSON.parse(evt.data);
                    console.log("evt: ", evt);
                    if (evt["fun"] == "buystock" && evt["status"] == "success") {
                        console.log('bought stock');
                    }
                }
            }
        }
    }, [ws, token, connected]);

    return (
        <div className="stockCard">
            <div className="text">
                <h1>{props.name}</h1>
                <ul>
                    <li>Price: ${props.price}</li>
                    <li>Shares: {props.shares}</li>
                </ul>
            </div>
            <div className="but">
                <Button onClick={handleBuy} variant="contained" color="primary">Buy Share</Button>
            </div>
        </div>
    )
}