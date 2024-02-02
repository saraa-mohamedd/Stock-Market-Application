import React from 'react'
import StockCard from '../../components/stockCard/stockCard.js'
import './styles.css'

import { useAuth } from '../../context/AuthContext';

export default function Stocks() {
    const { token } = useAuth();
    console.log(token);

    if (!token) {
        return <p>Please log in.</p>;
    }
    return (
        <>
        <div className="stockpageWrapper">
            <h2>View and purchase stocks here! With real-time stock price updates, monitor the market and make <span className="highlight"> informed decisions. </span></h2>
            <div className='stocksWrapper'>
                <StockCard name='AAPL' price='100' shares='10' />
                <StockCard name='Tesla' price='200' shares='5' />
                <StockCard name='Microsoft' price='150' shares='7' />
                <StockCard name='Amazon' price='300' shares='3' />
                <StockCard name='Google' price='250' shares='4' />
                <StockCard name='Facebook' price='175' shares='6' />
            </div>
        </div>
        </> 
        )
}
