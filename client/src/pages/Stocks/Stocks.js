import './styles.css'

import { useAuth, ProvideAuth } from '../../context/AuthContext';
import { useServer, ProvideServer } from '../../context/ServerContext';
import { useEffect, useState, React } from 'react';
import StockCard from '../../components/StockCard/stockCard.js'
import UserLogin from '../../components/UserLogin/UserLogin.js';
import ReactLoading from 'react-loading';
import { AreaChart, LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, Legend, Area} from 'recharts';

export default function Stocks() {
    const { token } = useAuth();
    const { ws, connected } = useServer();
    const [stocks, setStocks] = useState(["no stocks"]);
    const [stockPrices, setStockPrices] = useState(new Map());
    const [message, setMessage] = useState('');

    // handling receiving messages from the server
    useEffect(() => {
        if (ws && connected){
            ws.onmessage = evt => {
                if (evt.data){
                    evt = JSON.parse(evt.data);

                    if (evt["fun"] == "getallstocks" && evt["status"] == "success") {
                        setStocks(evt["data"]["stocks"]);
                    }
                    else if (evt["fun"] == "buystock"){
                        //set className of message box to display message for 1.5 seconds
                        let messageBox = document.getElementById('message-box');
                        messageBox.classList.add(evt["status"]);
                        messageBox.classList.remove("no-message");

                        setMessage(evt["message"]);
                        if (evt["status"] == "success") {
                            setStocks(evt["data"]["stocks"]);
                        }
                    } 
                }
            }
        }
    }, [ws, token, connected]);

    // fetching stocks every 2.5 seconds
    useEffect(() => {
        const myInterval = setInterval(fetchStocks, 2500);
        return () => {
            clearInterval(myInterval);
          };
    }, [ws, connected]);

    const fetchStocks = () => {
        let request = {};
        if (token!= "" && token!=null && token!=undefined){
            request = {
                "fun": "getallstocks",
                "data": {}
            }

            console.log('ws: ', ws);
            console.log('request: ', request);
            console.log('connected: ', connected)

            if (ws && connected){
                console.log('sending request');
                ws.send(JSON.stringify(request));
            }
        }
    }

    useEffect(() => {
        console.log('stocks in useeffect: ', stocks);
        let prices = new Map();
        stocks.forEach(stock => {
            let price = stock.price;
            let name = stock.company;
            if (stockPrices.has(name)) {
                let existingPrices = stockPrices.get(name);
                existingPrices.push({price: price});
                prices.set(name, existingPrices);
            }
            else{
                prices.set(name, [{price: price}]);
            }
            
        })
        setStockPrices(prices);
        console.log('stockPrices: ', stockPrices);
    }
    , [stocks]);

    // setting a token automatically refreshes the page, so this does not need to be in a useEffect
    if (!token) {
        return (
            <ProvideAuth>
                <ProvideServer>
                    <div className="stockpage-wrapper">
                        <h2>Please <span className="highlight">sign up</span> or <span className="highlight">log in</span> to access this page</h2>
                        <UserLogin />
                    </div>
                </ProvideServer>
            </ProvideAuth>   
        )
    }

    return (
        <ProvideAuth>
            <ProvideServer>
                {stocks.length && stocks[0] == "no stocks"?  
                    <div className="stockpage-wrapper">
                        <div className='loading-text'>Loading stocks...</div>
                        <ReactLoading type={'balls'} color={"#002349"} height={'10rem'} width={'15rem'} />
                    </div> 
                    :
                    <>
                    <div className="stockpage-wrapper">
                        <h2>View and purchase stocks here! With <span className="highlight">real-time</span> stock price updates, monitor the market and make <span className="highlight"> informed decisions. </span></h2>
                        <div id="message-box" className="no-message" >
                            {message}
                        </div>
                        <div className='stocks-container'>
                            {stocks.length > 0 ?
                                stocks.map((stock, index) => {
                                    return (
                                        <ProvideAuth>
                                            <ProvideServer>
                                                <StockCard key={stock.company} type={"buy"} name={stock.company} prices={stockPrices.has(stock.company) ? stockPrices.get(stock.company) : []} shares={stock.remainingShares} />
                                            </ProvideServer>
                                        </ProvideAuth>
                                    )
                                }):
                                <div className='loading-text'>No stocks available</div>}
                        </div>
                    </div></>
                }
            </ProvideServer>
        </ProvideAuth>
    )
}