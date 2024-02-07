import './styles.css'

import { useAuth, ProvideAuth } from '../../context/AuthContext.js';
import { useServer, ProvideServer } from '../../context/ServerContext.js';
import { useEffect, useState, React } from 'react';
import TransactionCard from '../../components/TransactionCard/TransactionCard.js'
import UserLogin from '../../components/UserLogin/UserLogin.js';
import ReactLoading from 'react-loading';

export default function Wallet() {
    const { token } = useAuth();
    const { ws, connected } = useServer();
    const [transactions, setTransactions] = useState([]);

    let request = {
        "fun": "gettransactions",
        "data": {
            "email": token,
        }
    }
    console.log('request: ', request);
    
    // handling receiving messages from the server
    useEffect(() => {
        if (ws && connected){
            ws.onmessage = evt => {
                if (evt.data){
                    evt = JSON.parse(evt.data);
                    if (evt["fun"] == "gettransactions" && evt["status"] == "success") {
                        console.log('got all user transactions');
                        console.log('transactions: ', evt["data"]["transactions"]);
                        setTransactions(evt["data"]["transactions"]);
                    }
                }
            }
        }
    }, [ws, token, connected]);
    
    // setting a token automatically refreshes the page, so this does not need to be in a useEffect
    if (!token) {
        return (
            <ProvideAuth>
                <ProvideServer>
                    <div className="walletpage-wrapper">
                        <h2>Please <span className="highlight">sign up</span> or <span className="highlight">log in</span> to access this page</h2>
                        <UserLogin />
                    </div>
                </ProvideServer>
            </ProvideAuth>   
        )
    }
    
    if (ws && connected){
        ws.send(JSON.stringify(request));
        console.log('sending request');
    }

    //sort transactions by date
    transactions.sort((a, b) => {
        return new Date(b.datetime) - new Date(a.datetime);
    });

    return (
        <ProvideAuth>
            <ProvideServer>
                {transactions.length == 0 ?  
                    <div className="transactionpage-wrapper">
                        <div className='loading-text'>Loading your transactions...</div>
                        <ReactLoading type={'balls'} color={"#002349"} height={'10rem'} width={'15rem'} />
                    </div>
                    :
                    <>
                    <div className="transactionpage-wrapper">
                        <h2>View your past transactions here!</h2>
                        <div className='transactions-container'>
                            {transactions.map((transaction, index) => {
                                return (
                                    <ProvideAuth>
                                        <ProvideServer>
                                            <TransactionCard key={transaction.stockCompany} type={transaction.type} name={transaction.stockCompany} price={transaction.price} datetime={transaction.datetime} />
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
