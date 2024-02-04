import React, { useState } from "react";
import { useAuth } from '../../context/AuthContext.js';
import {FaUser} from 'react-icons/fa';
import {MdMail} from 'react-icons/md';
import {RiLockPasswordFill} from 'react-icons/ri';
import './styles.css';

export default function UserLogin(){
    const { setAuth } = useAuth();
    const [token, setToken] = useState(null);
    // const navigate = useNavigate();

    const handleLogin = async () => {
        if (action === "Sign Up") {
            setAction('Log In');
            return;
        }
        // Replace this with your actual login logic
        const response = await test(email, password);
        console.log(response);
        setToken(response.token);
        setAuth(response.token);
        window.location.reload();
    };

    const [action, setAction] = useState('Sign Up');
    const [email, setEmail] = useState('');
    const [password, setPassword] = useState('');
    const [name, setName] = useState('');

    return (
        <>
        <div className="loginWrapper">
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
            <div className={action==="Log In" ?"submit inactive" : "submit"} onClick={()=>{setAction("Sign Up")}}>Sign Up</div>
            {/* send content of inputs to the handleLogin function... */}
            <div className={action==="Log In" ?"submit" : "submit inactive"} onClick={handleLogin}>Log In</div>
        </div>
        </div>
        </>
    );
}

//some api call to login
function test(email, password) {
    return new Promise((resolve) => {
        setTimeout(() => {
            resolve({ token: email + '1234'});    
            console.log('Logged in');
        }
        , 1000);
    }
    );
}