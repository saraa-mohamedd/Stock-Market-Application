import './styles.css';
import { AiFillDollarCircle } from 'react-icons/ai';
import UserLogin from '../../components/userLogin/UserLogin';
import { ProvideAuth, useAuth } from '../../context/AuthContext';
// import { useEffect, useState } from 'react';
import { ProvideServer, useServer } from '../../context/ServerContext';
import { useEffect } from 'react';


const Home = () => {
    const {ws} = useServer();
    const {token} = useAuth();

    //set token to change automatically when user logs in
    

    useEffect(() => {
        if (!ws) return;
        ws.onmessage = evt => {
            console.log('Received: ' + evt.data);
        }
     }, [ws]);

    //  useEffect(() => {
    //     if (!token) return;
    //     document.getElementById("login-register-box").innerHTML = `<p>Logged in as ${token}</p>`;
        
    //  }, [token]);

        
    return (
        <div className="homepage-wrapper" id='server-response'>
            <h1>QuickStock  <AiFillDollarCircle color='#002349' /></h1>
            <h2>Welcome to the <span className="app-name">future</span> of stock trading.</h2>
            <p>Stock trading made easy! Just one stop for viewing, buying, and selling stocks</p>
            <div id="login-register-box">
            {token ? <p>Logged in as {token}</p> : 
            <ProvideAuth>
                <ProvideServer>
                <UserLogin />
                </ProvideServer>
            </ProvideAuth>}
            </div>
            <button onClick={() => ws.send('Hello, Server!')}>Send</button>
            <h1 id="server-response">${}</h1>
        </div>
    )
}

export default Home;
