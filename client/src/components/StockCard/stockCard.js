import './styles.css'
import Button from '@material-ui/core/Button';
import { useAuth } from '../../context/AuthContext';
import { useEffect, useRef, useState } from 'react';
import { useServer } from '../../context/ServerContext';
import { AreaChart, LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, Area} from 'recharts';

export default function StockCard(props) {
    const {token} = useAuth();
    const {ws, connected} = useServer();
    const [prices, setPrices] = useState([]);
    const chartRef = useRef();

    useEffect(() => {
        if (props.prices.length > 20){
            while (props.prices.length > 20){
                props.prices.shift();
            }
        }
        setPrices(props.prices);
        console.log("stock card prices: ", props.prices);
    }, [props.prices.length]);

    const handleBuy = () => {
        let request = {
            "fun": "buystock",
            "data": {
                "email": token,
                "company": props.name,
                "price": props.prices[props.prices.length - 1].price
            }
        }
        console.log('request: ', request);
        if (ws && connected)
            ws.send(JSON.stringify(request));
    }

    const handleSell = () => {
        let request = {
            "fun": "sellstock",
            "data": {
                "email": token,
                "company": props.name,
                "price": props.prices[props.prices.length - 1].price
            }
        }
        console.log('request: ', request);
        if (ws && connected){
            console.log("sending sell request")
            ws.send(JSON.stringify(request));
        }
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
                    else if (evt["fun"] == "sellstock" && evt["status"] == "success") {
                        console.log('sold stock');
                    }
                }
            }
        }
    }, [ws, token, connected]);

    return (
        <div className="stockCard">
            <div className="text">
                <h1>{props.name}</h1>
                <AreaChart width={350} height={300} data={prices} key={`ls_${props.prices.length}`}
                >
                    <defs>
                        <linearGradient id="fillGradient" x1="0" y1="-1" x2="0" y2="1">
                            <stop offset="5%" stopColor="#002349" stopOpacity={1} />
                            <stop offset="95%" stopColor="#FFFFFF" stopOpacity={1} />
                        </linearGradient>
                    </defs>
                    <CartesianGrid strokeDasharray="5 5" />
                    <XAxis dataKey="x"/>
                    <YAxis tick={false} width={1}/>  
                    <Tooltip />
                    <Area
                        type="natural"
                        dataKey="price"
                        isAnimationActive={true}
                        animationDuration={0.01}
                        stroke="#002349"
                        strokeWidth={4}
                        fill="url(#fillGradient)"
                        dot={false}
                    />
                </AreaChart>
                <ul>
                    <li><span className="label">Price:</span> ${props.prices.length > 0 ? props.prices[props.prices.length - 1].price : 0}</li>
                    <li><span className='label'>Shares:</span> {props.shares}</li>
                </ul>
            </div>
                <div className={props.type == "buy" ? "buy-button" : "sell-button"} onClick={props.type == "buy" ? handleBuy : handleSell}>{props.type == "buy" ? "Buy Share" : "Sell Share"}</div>
        </div>
    )
}