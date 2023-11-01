# Custom Traceroute

## Overview
This project is a custom implementation of the traceroute command in C, enhanced to provide additional features such as displaying bandwidth, geolocation information and reporting the status of ports for each router along the network path.

## Features
- Custom traceroute command with extended capabilities.
- Real-time bandwidth estimation for each hop.
- Geolocation information ( city, region etc.) for each router.
- Status of ports of each router

## Installation
1. Clone the repository:
   ```
   git clone https://github.com/Anupriya-Dey/Custom-Traceroute.git
   ```

2. Compile the code:
   ```
   cd Custom-Traceroute
   gcc main.c utils.c geolocation.c -lcurl -ljansson -Wall -Werror -Wextra -I. -o customTraceroute
   sudo ./customTraceroute <hostname>
   ```

## Contributors
1. Anupriya Dey
2. Himani Panwar
3. Komal Gupta
4. Naqiyah Kagzi