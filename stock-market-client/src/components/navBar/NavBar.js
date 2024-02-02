import './styles.css';
import { Link, useMatch, useResolvedPath } from "react-router-dom";


export default function NavBar() {
    return (
        <nav className="navbar">
            <Link className="siteTitle" to="/">Stock Market</Link>
        <ul>
            <li>
            <CustomLink to="/stocks">Stocks</CustomLink>
            </li>
            <li>
            <CustomLink to="/wallet">My Wallet</CustomLink>
            </li>
        </ul>
        </nav>
    );
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