import React, { useState } from "react";
import { ProvideAuth, useAuth } from '../../context/AuthContext.js';
import {FaUser} from 'react-icons/fa';
import {MdMail} from 'react-icons/md';
import {RiLockPasswordFill} from 'react-icons/ri';
import { ProvideServer, useServer } from '../../context/ServerContext.js';
import { useEffect } from 'react';
import './styles.css';

export default function UserLogin(){
    const { setAuth } = useAuth();
    const { ws } = useServer();
    const [token, setToken] = useState(null);

    const [action, setAction] = useState('Sign Up');
    const [email, setEmail] = useState('');
    const [password, setPassword] = useState('');
    const [name, setName] = useState('');

    const handleLogin = async () => {
        if (action === "Sign Up") {
            setAction('Log In');
            return;
        }

        let request = {
            "fun": "login",
            "data": {
                "email": email,
                "password": password
            }
        }

        ws.send(JSON.stringify(request));
    };

    const handleRegister = async () => {    
        if (action === "Log In") {
            setAction('Sign Up');
            return;
        }

        let request = {
            "fun": "register",
            "data": {
                "email": email,
                "password": password,
                "fullname": name
            }
        }

        ws.send(JSON.stringify(request));
    }

    useEffect(() => {
        if (ws){
            console.log('ws: ', ws);
            ws.onmessage = evt => {
                evt = JSON.parse(evt.data);
                console.log("evt: ", evt);
                if (evt["fun"] == "login" && evt["status"] == "success") {
                    console.log('Logged in');
                    setAuth(evt["data"]["email"]);
                    setToken(evt["data"]["email"]);
                    window.location.reload();
                }
                console.log('Received: ' + JSON.stringify(evt));
            }

        }
     }, [ws]);

    return (
        <ProvideAuth>
            <ProvideServer>
        <div className="login-wrapper">
            <h2><span>{action}</span> to QuickStock</h2>
            <div className="underline"></div>
            {/* <LoginForm handleLogin={handleLogin} /> */}
            <div className = "inputs">
                {action ==="Log In"? <div></div> : <div className = "input">
                    <FaUser size={20}/>
                    <input type="text" placeholder="Full Name" onChange={(event)=>setName(event.target.value)}/>
                </div>}
                <div className = "input">
                    <MdMail size={20}/>
                    <input type="text" placeholder="Email" onChange={(e)=>setEmail(e.target.value)}/>
                </div>
                <div className = "input">
                    <RiLockPasswordFill size={20}/>
                    <input type="password" placeholder="Password" onChange={(event)=>setPassword(event.target.value)} />
                    </div>
            </div>
            {action ==="Sign Up"? <div></div> : <div className="forgot-password">Forgot Password? <span>Click Here!</span></div>}
            <div className="submit-container">
                <div className={action==="Log In" ?"submit inactive" : "submit"} onClick={handleRegister}>Sign Up</div>
                {/* send content of inputs to the handleLogin function... */}
                <div className={action==="Log In" ?"submit" : "submit inactive"} onClick={handleLogin}>Log In</div>
            </div>
        </div>
        </ProvideServer>
        </ProvideAuth>
    );
}