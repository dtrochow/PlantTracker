import { Flex, Text } from "@chakra-ui/react";

interface TopAppBarProps {
    appName: string;
}


function TopAppBar({ appName }: TopAppBarProps) {
    return (
        <Flex
            as="nav"
            align="center"
            justify="space-between"
            wrap="wrap"
            padding="1.5rem"
            bg="teal.500"
            color="white"
        >
            <Flex align="center" mr={5}>
                <Text fontSize="lg" fontWeight="bold">
                    {appName}
                </Text>
            </Flex>
        </Flex>
    );
}

export default TopAppBar;