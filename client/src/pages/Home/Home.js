import './styles.css';
import { AiFillDollarCircle } from 'react-icons/ai';
import UserLogin from '../../components/userLogin/UserLogin';
import { ProvideAuth, useAuth } from '../../context/AuthContext';
import { ProvideServer, useServer } from '../../context/ServerContext';
import { useEffect, useState } from 'react';


const Home = () => {
    const {ws, connected} = useServer();
    const {token} = useAuth();
    const [name, setName] = useState('');

    useEffect(() => {
        let request = {};
        if (token){
            request = {
                "fun": "getdetails",
                "data": {
                    "email": token
                }
            }
            console.log('request: ', request);
        }

        if (ws && connected){
            ws.send(JSON.stringify(request));  
            console.log('ws: ', ws);

            ws.onmessage = evt => {
                evt = JSON.parse(evt.data);
                console.log("evt: ", evt);
                if (evt["fun"] == "getdetails" && evt["status"] == "success") {
                    console.log('got user details');
                    setName(evt["data"]["fullname"]);
                }
            }

        }
     }, [ws, token, connected]);

     useEffect(() => {
        console.log('home sees token change: ', token);
        
        // ws.send(JSON.stringify(request));        
     }, [token]);

        
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
                    <h1 id="server-response">${}</h1>
                </div>
            </ProvideServer>
        </ProvideAuth>
        
    )
}

export default Home;
