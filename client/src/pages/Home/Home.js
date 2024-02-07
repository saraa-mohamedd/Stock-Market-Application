import './styles.css';
import { AiFillDollarCircle } from 'react-icons/ai';
import UserLogin from '../../components/UserLogin/UserLogin';
import { ProvideAuth, useAuth } from '../../context/AuthContext';
import { ProvideServer, useServer } from '../../context/ServerContext';
import { useEffect, useState } from 'react';


const Home = () => {
    const {ws, connected} = useServer();
    const {token} = useAuth();
    const [name, setName] = useState('');

    // upon receiving a message from the server
    useEffect(() => {
        if (ws && connected){
            ws.onmessage = evt => {
                if (evt.data){
                    evt = JSON.parse(evt.data);
                    if (evt["fun"] == "getdetails" && evt["status"] == "success") {
                        setName(evt["data"]["fullname"]);
                    }
                }
            }
        }
     }, [ws, token, connected]);

    // get user details if token is present and connected to server
    useEffect(() => {
        let request = {};
        if (token!= "" && token!=null && token!=undefined){
            request = {
                "fun": "getdetails",
                "data": {
                    "email": token
                }
            }

            if (ws && connected){
                ws.send(JSON.stringify(request));
            }
        }
    }, [token, connected]);


    return (
        <ProvideAuth>
            <ProvideServer>
                <div className="homepage-wrapper" id='server-response'>
                    <h1>QuickStock  <AiFillDollarCircle color='#002349' /></h1>
                    <h2>Welcome to the <span className="app-name">future</span> of stock trading.</h2>
                    <p>Stock trading made easy! Just one stop for viewing, buying, and selling stocks</p>
                    <div id="login-register-box">
                        {token ? <p>Logged in as {name}</p> : 
                            <UserLogin />}
                    </div>
                </div>
            </ProvideServer>
        </ProvideAuth>
        
    )
}

export default Home;
