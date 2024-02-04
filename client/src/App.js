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
    <ProvideAuth>
    <NavBar />
    <div className='container'>
      <ProvideAuth>
        <ProvideServer>
      <Routes>
        <Route path="/" element={<Home />} />
        <Route path="/stocks" element={<Stocks />} />
        <Route path="/wallet" element={<Wallet />} />
      </Routes>
      </ProvideServer>
      </ProvideAuth>
    </div>
    </ProvideAuth>
    </>
  );
}
 

export default App;
