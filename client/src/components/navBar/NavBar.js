import './styles.css';
import { Link, useMatch, useResolvedPath } from "react-router-dom";
import { AiOutlineStock } from "react-icons/ai";
import {MdLogout} from "react-icons/md";
import { useAuth } from '../../context/AuthContext.js';

export default function NavBar() {
    const { token } = useAuth();
    console.log(token);

    return (
        <nav className="navbar">
            <Link className="siteTitle" to="/"><AiOutlineStock size={55}/>QuickStock</Link>
        <ul>
            <CustomLink to="/stocks">Stocks</CustomLink>
            <CustomLink to="/wallet">My Wallet</CustomLink>
            {token ? <MdLogout className="logout-icon" size={50} onClick={handleLogOut} ></MdLogout> : <div></div>}
        </ul>
        </nav>
    );
    }


function handleLogOut(){
    localStorage.removeItem('session');
    window.location.reload();
}

function CustomLink({to, children, ...props}) {
    // const path = window.location.pathname;
    const resolvedPath = useResolvedPath(to);
    const isActive = useMatch({path: resolvedPath.pathname, end: true})
    
    return (
        <li className = {isActive ? "active" : ""}>
            <Link to={to} {...props}>{children}</Link>
        </li>
    );
    }