import NavBar from './components/navBar/NavBar';
import Home from './pages/Home/Home';
import Stocks from './pages/Stocks/Stocks';
import Wallet from './pages/Wallet';
import { Route, Routes, Link } from "react-router-dom";
import { ProvideAuth, useAuth } from './context/AuthContext';
import { ProvideServer, ServiceProvider } from './context/ServerContext';

import './styles/globalstyles.css';

function App() {
  return (
    <>
    <ProvideServer>
      <ProvideAuth>
        <NavBar />
        <div className='container'>
            <ProvideAuth>
              <Routes>
                <Route path="/" element={<ProvideAuth><ProvideServer><Home /></ProvideServer></ProvideAuth>} />
                <Route path="/stocks" element={<Stocks />} />
                <Route path="/wallet" element={<Wallet />} />
              </Routes>
            </ProvideAuth>
        </div>
      </ProvideAuth>
    </ProvideServer>
    </>
  );
}
 

export default App;
