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
    const [message, setMessage] = useState(''); // message to display to user

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
                if (evt["fun"] == "login"){
                    if (evt["status"] == "success") {
                        console.log('Logged in');
                        setAuth(evt["data"]["email"]);
                        setToken(evt["data"]["email"]);
                        window.location.reload();
                    }
                    else {
                        // set and display error message
                        setMessage(evt["message"]);
                        document.getElementById("message-box").classList.remove("no-message");
                        document.getElementById("message-box").classList.remove("success");
                        document.getElementById("message-box").classList.add("error");
                    }
                }
                else if (evt["fun"] == "register"){
                    if (evt["status"] == "success") {
                        console.log('Registered');

                        // set and display success message
                        setMessage(evt["message"]);
                        document.getElementById("message-box").classList.remove("no-message");
                        document.getElementById("message-box").classList.remove("error");
                        document.getElementById("message-box").classList.add("success");
                    }
                    else {

                        // set and display error message
                        setMessage(evt["message"]);
                        document.getElementById("message-box").classList.remove("no-message");
                        document.getElementById("message-box").classList.remove("success");
                        document.getElementById("message-box").classList.add("error");
                    }
                }
                console.log('Received: ' + JSON.stringify(evt));
            }

        }
     }, [ws]);

    return (
        <ProvideAuth>
            <ProvideServer>
        <div className="login-wrapper">
            <h2><span className="highlight">{action}</span> to QuickStock</h2>
            <div className="underline"></div>
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
            {action ==="Sign Up"? <div></div> : <div className="forgot-password">Forgot Password? <span>Click Here!</span></div>}
            </div>
            <div className="submit-container">
                <div className={action==="Log In" ?"submit inactive" : "submit"} onClick={handleRegister}>Sign Up</div>
                <div className={action==="Log In" ?"submit" : "submit inactive"} onClick={handleLogin}>Log In</div>
            </div>
        </div>
        <div id="message-box" className="no-message" >
            {message}
        </div>
        </ProvideServer>
        </ProvideAuth>
    );
}