import './styles.css';
import { AiFillDollarCircle } from 'react-icons/ai';
import UserLogin from '../../components/userLogin/UserLogin';
import { ProvideAuth, useAuth } from '../../context/AuthContext';


export default function Home() {
    const {token} = useAuth();

    return (
        <div className="homepageWrapper">
            <h1>QuickStock  <AiFillDollarCircle color='#002349' /></h1>
            <h2>Welcome to the <span className="appName">future of stock trading.</span></h2>
            <p>Stock trading made easy! Just one stop for viewing, buying, and selling stocks</p>
            {token ? <p>Logged in as {token}</p> : 
            <ProvideAuth>
                <UserLogin />
            </ProvideAuth>}
        </div>
    )
}