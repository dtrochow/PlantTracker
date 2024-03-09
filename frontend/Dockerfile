
# Use the official Node.js image as the base image
FROM node:21-slim as build

# Set the working directory inside the container
WORKDIR /app

# Copy the package.json and yarn.lock files to the container
COPY package*.json yarn.lock ./

# Install the dependencies using yarn
RUN yarn install

# Copy the rest of the application code to the container
COPY . .

# Build the production version of the app
RUN yarn build
