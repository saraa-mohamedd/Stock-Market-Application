import { createContext, useState , useContext } from "react";

export const AuthContext = createContext();

export const ProvideAuth = ({ children }) => {
    const getSession = () => {
        return JSON.parse(localStorage.getItem('session'));
    };

    const setSessionInLocalStorage = (token) => {
        localStorage.setItem('session', JSON.stringify(token))
        return true
    };

    const auth = getSession();
    const [session, setSession] = useState(auth || '');
    const setAuth = (token) => {
        setSession(token);
        setSessionInLocalStorage(token);
    };
    const token = session;
    console.log(token);
    return (
        <AuthContext.Provider value={{ token, setAuth }}>
            {children}
        </AuthContext.Provider>
    )
};

export const useAuth = () => useContext(AuthContext);

