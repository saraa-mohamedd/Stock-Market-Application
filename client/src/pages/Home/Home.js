import './styles.css';
import { AiFillDollarCircle } from 'react-icons/ai';
import UserLogin from '../../components/userLogin/UserLogin';
import { ProvideAuth, useAuth } from '../../context/AuthContext';
// import { useEffect, useState } from 'react';
import { useServer } from '../../context/ServerContext';


const Home = () => {
    const {ws} = useServer();
    const {token} = useAuth();

    // render tje text from the server
    ws.onmessage = evt => {
        console.log('Received: ' + evt.data);
        document.getElementById('server-response').innerHTML = evt.data;
    };
    return (
        <div className="homepage-wrapper">
            <h1>QuickStock  <AiFillDollarCircle color='#002349' /></h1>
            <h2>Welcome to the <span className="app-name">future of stock trading.</span></h2>
            <p>Stock trading made easy! Just one stop for viewing, buying, and selling stocks</p>
            {token ? <p>Logged in as {token}</p> : 
            <ProvideAuth>
                <UserLogin />
            </ProvideAuth>}
            <button onClick={() => ws.send('Hello, Server!')}>Send</button>
            <h1 id="server-response">${}</h1>
        </div>
    )
}

export default Home;
