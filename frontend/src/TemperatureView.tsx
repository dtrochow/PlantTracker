import { useEffect, useState } from 'react';
import { Button, Center, Flex, Text } from '@chakra-ui/react';
import ky from 'ky';

interface TemperatureResponse {
    temperature: number;
}

const TemperatureView = () => {
    const [temperature, setTemperature] = useState<number | undefined>();
    const [isLoading, setIsLoading] = useState(false);

    useEffect(() => {
        fetchTemperature();
    }, []);

    const fetchTemperature = async () => {
        const timer = setTimeout(() => setIsLoading(true), 100);
        try {
            const data = await ky.get('/api/temperature').json<TemperatureResponse>();
            clearTimeout(timer);
            setTemperature(data.temperature);
        } catch (error) {
            clearTimeout(timer);
            console.error('Error fetching temperature:', error);
        }

        setIsLoading(false);
    };

    return (
        <Flex justifyContent="center" alignItems="center" height="100vh">
            <Center flexDirection="column">
                <Text fontSize="8xl" fontWeight="bold" mb={4}>
                    {isLoading ? 'Loading...' : temperature ? `${temperature}°C` : '\u00A0' /* Non-breaking space */}
                </Text>
                <Button colorScheme="teal" size="lg" onClick={fetchTemperature}>
                    Update Temperature
                </Button>
            </Center>
        </Flex>
    );
};

export default TemperatureView;