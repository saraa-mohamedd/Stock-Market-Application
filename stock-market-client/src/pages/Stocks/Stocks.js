import React from 'react'
import StockCard from '../../components/stockCard/stockCard.js'
import './styles.css'

export default function Stocks() {
    return (
        <>
        <div className="pageWrapper">
            <h1>View and purchase stocks here</h1>
            <div className='stocksWrapper'>
                <StockCard className="stocks" name="Sara" />
            </div>
        </div>
        </> )
}
