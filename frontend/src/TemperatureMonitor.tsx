import { useState } from 'react';
import { Button, Center, Flex, Text } from '@chakra-ui/react';
import ky from 'ky';

interface TemperatureResponse {
    temperature: number;
}

const TemperatureMonitor = () => {
    const [temperature, setTemperature] = useState<number | null>(null);

    const fetchTemperature = async () => {
        try {
            const data = await ky.get('/api/temperature').json<TemperatureResponse>();
            setTemperature(data.temperature);
        } catch (error) {
            console.error('Error fetching temperature:', error);
        }
    };

    return (
        <Flex justifyContent="center" alignItems="center" height="100vh">
            <Center flexDirection="column">
                <Text fontSize="4xl" fontWeight="bold" mb={4}>
                    {temperature !== null ? `${temperature}°C` : 'Loading...'}
                </Text>
                <Button colorScheme="blue" onClick={fetchTemperature}>
                    Update Temperature
                </Button>
            </Center>
        </Flex>
    );
};

export default TemperatureMonitor;