export default function Wallet() {
    return (
        <div className="wallet">
            <h1>My Wallet</h1>
            <p>Balance: $5000</p>
            <ul>
                <li>Stock: AAPL, Shares: 10, Price: $1500</li>
                <li>Stock: TSLA, Shares: 5, Price: $2500</li>
                <li>Stock: AMZN, Shares: 2, Price: $3000</li>
            </ul>
        </div>
    )
}