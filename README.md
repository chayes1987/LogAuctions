# LogAuctions
This is the log auctions service for my FYP. It is written in C++. It uses a 0mq binding.
It is responsible for monitoring all auctions.

## License

None

## Setting up StartAuction service on AWS

- Created AWS EC2 Windows instance
- Connected with RDP and .pem keyfile
- Copied Service build to instance
- Configure Windows Firewall to allow port access for 0mq

- Service runs and works as expected

## Problems encountered

- Difficulty in setting up 0mq in C++, linking libraries etc

