//create web socket instance to be used accross app
import React from 'react';
import { createContext, useState, useEffect} from 'react';

export const ServerContext = createContext();

export const ProvideServer = ({ children }) => {
    const [ws, setWs] = useState(null);
    const [connected, setConnected] = useState(false);
   
    useEffect(() => {
       setWs(new WebSocket('ws://localhost:9002'));

         return () => {
                ws && ws.close();
            }
    }, []);
   
    useEffect(() => {
       if (!ws) return;
   
       ws.onopen = () => {
         console.log('connected');
         setConnected(true);
       };
   
       ws.onerror = error => {
         console.log('WebSocket Error: ' + error);
       };
   
       ws.onclose = () => {
         console.log('disconnected');
       };
    }, [ws]);


    return (
        <ServerContext.Provider value={{ ws, connected }}>
            {children}
        </ServerContext.Provider>
    )
}

export const useServer = () => React.useContext(ServerContext);