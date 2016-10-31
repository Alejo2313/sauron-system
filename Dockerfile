FROM node

# Create app directory
RUN mkdir /server
WORKDIR /server

# Bundle app source
COPY sauron-server /server/

# Install npm and bower dependencies
RUN npm install

EXPOSE 4000
CMD node ./bin/www
