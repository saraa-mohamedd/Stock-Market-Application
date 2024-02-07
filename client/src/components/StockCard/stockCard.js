import './styles.css'
import Button from '@material-ui/core/Button';
import { useAuth } from '../../context/AuthContext';
import { useEffect, useRef, useState } from 'react';
import { useServer } from '../../context/ServerContext';
import { AreaChart, LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, Area} from 'recharts';
import ApexChart from 'react-apexcharts';
import Chart from 'react-apexcharts';

export default function StockCard(props) {
    const {token} = useAuth();
    const {ws, connected} = useServer();
    const [prices, setPrices] = useState([]);

    const series =[
        {
            name: "Price",
            data: prices.map(p => p.price)
        }
    ]

    const options = {
        chart: {
            id: "basic-bar",
            type: "line",
            animation: {
                enabled: true,
                easing: 'linear',
                dynamicAnimation: {
                    speed: 1000
                }
            },
            toolbar: {
                show: false
            }
        },
        dataLabels: {
            enabled: false
        },
        stroke: {
            curve: 'straight',
            colors: ['#0f52ba'],
            width: 3,
        },
        xaxis: {
            categories: prices.map(p => p.x)
        },
        fill: {
            colors: ['#0f52ba']
          }

    }

    useEffect(() => {
        if (props.prices.length > 20){
            while (props.prices.length > 20){
                props.prices.shift();
            }
        }
        setPrices(props.prices);
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
        console.log('buy   request: ', request);
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
                <Chart
                    options={options}
                    series={series}
                    type="area"
                    width={350}
                    height={300}></Chart>
                <ul>
                    <li><span className="label">Price:</span> ${props.prices.length > 0 ? props.prices[props.prices.length - 1].price : 0}</li>
                    <li><span className='label'>Shares:</span> {props.shares}</li>
                </ul>
            </div>
                <div className={props.type == "buy" ? "buy-button" : "sell-button"} onClick={props.type == "buy" ? handleBuy : handleSell}>{props.type == "buy" ? "Buy Share" : "Sell Share"}</div>
        </div>
    )
}