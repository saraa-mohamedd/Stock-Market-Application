import './styles.css'
import Button from '@material-ui/core/Button';

export default function StockCard(props) {
    return (
        <div className="stockCard">
            <div className="text">
                <h1>{props.name}</h1>
                <ul>
                    <li>Price: ${props.price}</li>
                    <li>Shares: {props.shares}</li>
                </ul>
            </div>
            <div className="but">
                <Button variant="contained" color="primary">Buy Share</Button>
            </div>
        </div>
    )
}