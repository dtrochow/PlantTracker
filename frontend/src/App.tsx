import './App.css'
import TemperatureMonitor from './TemperatureMonitor'
import TopAppBar from './TopAppBar'

function App() {
  return (
    <>
      <TopAppBar appName='PlantTracker'/>
      <TemperatureMonitor />
    </>
  )
}

export default App
