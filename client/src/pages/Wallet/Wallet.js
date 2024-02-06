import './styles.css'

import { useAuth, ProvideAuth } from '../../context/AuthContext.js';
import { useServer, ProvideServer } from '../../context/ServerContext.js';
import { useEffect, useState, React } from 'react';
import StockCard from '../../components/stockCard/stockCard.js'
import UserLogin from '../../components/userLogin/UserLogin.js';
import ReactLoading from 'react-loading';

export default function Wallet() {
    const { token } = useAuth();
    const { ws, connected } = useServer();
    const [stocks, setStocks] = useState([]);
    const [stockPrices, setStockPrices] = useState(new Map());

    // handling receiving messages from the server
    useEffect(() => {
        if (ws && connected){
            ws.onmessage = evt => {
                if (evt.data){
                    evt = JSON.parse(evt.data);
                    // console.log("evt: ", evt);
                    if (evt["fun"] == "getuserstocks" && evt["status"] == "success") {
                        // console.log('got all user stocks');

                        setStocks(evt["data"]["stocks"]);
                    }
                    else if (evt["fun"] == "sellstock" && evt["status"] == "success") {
                        console.log('sold stock');
                        setStocks(evt["data"]["stocks"]);
                    }
                }
            }
        }
    }, [ws, token, connected]);

    // fetching user stocks every 2.5 seconds
    useEffect(() => {
        const myInterval = setInterval(fetchUserStocks, 2500);
        return () => {
            clearInterval(myInterval);
          };
    }, [ws, connected]);

    const fetchUserStocks = () => {
        let request = {};
        if (token!= "" && token!=null && token!=undefined){
            request = {
                "fun": "getuserstocks",
                "data": {
                    "email": token
                }
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
                    <div className="walletpage-wrapper">
                        <h2>Please <span className="highlight">sign up</span> or <span class="highlight">log in</span> to access this page</h2>
                        <UserLogin />
                    </div>
                </ProvideServer>
            </ProvideAuth>   
        )
    }

    return (
        <ProvideAuth>
            <ProvideServer>
                {stocks.length == 0 ?  
                    <div className="walletpage-wrapper">
                        <div className='loading-text'>Loading your stocks...</div>
                        <ReactLoading type={'balls'} color={"#002349"} height={'10rem'} width={'15rem'} />
                    </div>
                    :
                    <>
                    <div className="walletpage-wrapper">
                        <h2>View your stocks here! With real-time stock price updates, <span class="highlight">monitor</span> and <span className='highlight'>sell</span> your stocks.</h2>
                        <div className='stocks-container'>
                            {stocks.map((stock, index) => {
                                return (
                                    <ProvideAuth>
                                        <ProvideServer>
                                            <StockCard key={stock.company} type={"sell"} name={stock.company} prices={stockPrices.has(stock.company) ? stockPrices.get(stock.company) : []} shares={stock.remainingShares} />
                                        </ProvideServer>
                                    </ProvideAuth>
                                )
                            })}
                        </div>
                    </div></>
                }
            </ProvideServer>
        </ProvideAuth>
    )
}
