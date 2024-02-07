import './styles.css'

export default function TransactionCard(props) {

    let date = new Date(props.datetime);
    let dateStr = date.toDateString();
    let timeStr = date.toLocaleTimeString();

    let spanclass = props.type == 'buy' ? 'bought' : 'sold';

    return (
        <div className="transaction-card">
            <h4 id="date-time">{dateStr} {timeStr}</h4>
            <h3><span className={spanclass}>{props.type == 'buy' ? "Bought " : "Sold "}</span> 
                { props.name} stock for 
                <span className={spanclass}>{props.type == 'buy' ? " -" : " +"}${props.price}</span></h3>
        </div>
    )
}