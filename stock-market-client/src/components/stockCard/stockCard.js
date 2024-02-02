import './styles.css'

export default function StockCard(props) {
    return (
        <div className="stockCard">
            <h1>Stock: {props.name}</h1>
            <ul>
                <li>Price: {props.price}</li>
                <li>Shares: {props.shares}</li>
            </ul>
        </div>
    )
}