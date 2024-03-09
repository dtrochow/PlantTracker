import './App.css'
import TemperatureView from './TemperatureView'
import TopAppBar from './TopAppBar'

function App() {
  return (
    <>
      <TopAppBar appName='PlantTracker'/>
      <TemperatureView />
    </>
  )
}

export default App
