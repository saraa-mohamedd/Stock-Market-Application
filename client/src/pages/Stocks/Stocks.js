import './styles.css'

import { useAuth, ProvideAuth } from '../../context/AuthContext';
import { useServer, ProvideServer } from '../../context/ServerContext';
import { useEffect, useState, React } from 'react';
import StockCard from '../../components/StockCard/stockCard.js'
import UserLogin from '../../components/UserLogin/UserLogin.js';
import ReactLoading from 'react-loading';

export default function Stocks() {
    const { token } = useAuth();
    const { ws, connected } = useServer();
    const [stocks, setStocks] = useState(["no stocks"]);
    const [stockPrices, setStockPrices] = useState(new Map());

    // handling receiving messages from the server
    useEffect(() => {
        if (ws && connected){
            ws.onmessage = evt => {
                if (evt.data){
                    evt = JSON.parse(evt.data);

                    if (evt["fun"] == "getallstocks" && evt["status"] == "success") {
                        setStocks(evt["data"]["stocks"]);
                    }
                    else if (evt["fun"] == "buystock" && evt["status"] == "success") {
                        setStocks(evt["data"]["stocks"]);
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

    // function to fetch stocks
    const fetchStocks = () => {
        let request = {};
        if (token!= "" && token!=null && token!=undefined){
            request = {
                "fun": "getallstocks",
                "data": {}
            }

            if (ws && connected){
                console.log('sending request');
                ws.send(JSON.stringify(request));
            }
        }
    }

    // update stock prices in price map (to be sent to separate stock cards)
    useEffect(() => {
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