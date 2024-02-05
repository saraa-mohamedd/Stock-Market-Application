import './styles.css'

import { useAuth, ProvideAuth } from '../../context/AuthContext';
import { useServer, ProvideServer } from '../../context/ServerContext';
import { useEffect, useState, React } from 'react';
import StockCard from '../../components/stockCard/stockCard.js'
import UserLogin from '../../components/userLogin/UserLogin.js';
import ReactLoading from 'react-loading';

export default function Stocks() {
    const { token } = useAuth();
    const { ws, connected } = useServer();
    const [stocks, setStocks] = useState([]);
    const [stockpagewrapper, setStockPageWrapper] = useState(<div></div>)

    // handling receiving messages from the server
    useEffect(() => {
        if (ws && connected){
            ws.onmessage = evt => {
                if (evt.data){
                    evt = JSON.parse(evt.data);
                    console.log("evt: ", evt);
                    if (evt["fun"] == "getallstocks" && evt["status"] == "success") {
                        console.log('got all stocks');

                        setStocks(evt["data"]["stocks"]);
                    }
                    else if (evt["fun"] == "buystock" && evt["status"] == "success") {
                        console.log('bought stock');
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

    // rendering page either loading or with stocks depending on if stocks have been fetched
    useEffect(() => {
        if (stocks.length == 0) {
            setStockPageWrapper(
                <div className="stockpage-wrapper">
                    <div className='loading-text'>Loading stocks...</div>
                    <ReactLoading type={'balls'} color={"#002349"} height={'10rem'} width={'15rem'} />
                </div>
            )
        }
        else {
           setStockPageWrapper(
            <>
            <div className="stockpage-wrapper">
                <h2>View and purchase stocks here! With real-time stock price updates, monitor the market and make <span className="highlight"> informed decisions. </span></h2>
                <div className='stocks-container'>
                    {stocks.map((stock, index) => {
                        return (
                            <ProvideAuth>
                                <ProvideServer>
                                    <StockCard key={stock.company} name={stock.company} price={stock.price} shares={stock.remainingShares} />
                                </ProvideServer>
                            </ProvideAuth>
                        )
                    })}
                </div>
            </div></>
           )
        }

    }, [stocks]);

    // setting a token automatically refreshes the page, so this does not need to be in a useEffect
    if (!token) {
        return (
            <ProvideAuth>
                <ProvideServer>
                    {stockpagewrapper}
                    <div className="stockpage-wrapper">
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
                {stockpagewrapper}
            </ProvideServer>
        </ProvideAuth>
    )
}
