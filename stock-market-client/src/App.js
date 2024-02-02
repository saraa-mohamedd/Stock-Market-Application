import NavBar from './components/navBar/NavBar';
import Stocks from './pages/Stocks/Stocks';
import Wallet from './pages/Wallet';
import { Route, Routes } from "react-router-dom";
import './styles/globalstyles.css';

function App() {
  return (
    <>
    <NavBar />
    <div className='container'>
      <Routes>
        <Route path="/" element={<Stocks />} />
        <Route path="/stocks" element={<Stocks />} />
        <Route path="/wallet" element={<Wallet />} />

      </Routes>
    </div>
    </>
  );
}

export default App;
