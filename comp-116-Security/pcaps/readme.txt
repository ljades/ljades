Louis Ades

set1.pcap:

1. There are 1503 packets in this set.

2. FTP (File Transfer Protocol) was used to transfer files from PC to Server.

3. The data is in plaintext, unencrypted and easy to access. There is no
	security wall for accessing the transferred files, and so they are
	available to anyone with the pcap files to extract seamlessly.

4. The secure alternative is SFTP, (SSH File Transfer Protocol, or Secure File
	Transfer Protocol).

5. 67.23.79.113

6. Username: ihackpineapples
	Password: rockyou1

7. There were 4 files transferred.

8. smash.txt, BvzjaN-IQAA3XG7.jpg, BvgT9p2IQAEEoHu.jpg, BjN-O1hCAAAZbiq.jpg

9. They are now located in the repo, in the pcaps folder.


set2.pcap

10. There are 77882 packets in set2.pcap

11. First pair: USER: chris@digitalinterlude.com  PASS: Volrathw69
	Only one found. However, I did find that his username on the site is
		chric2k

12. I used ettercap and grep to format then filter out data and packets with 
the plaintext "USER:" in them. Searches such as "user" and "User" yielded no
results and "USER" without the colon yieled no extra results either.

13. First pair: 
	Protocol: POP. Server IP: 75.126.75.131. Domain name: stamps.com.
	Port number: 110

14. Access was successfully granted to the first pair.

15. I filtered TCP streams for those with the source IP address. I then checked
	the message sent by the server, and it said that authentication was
	cleared.

16. There are two pieces of advice I'd give. For starters, try to access
	websites such as this on more secure networks. Secure your own home
	network with a strong password so that your traffic can't be intercepted
	so easily. Additionally, access HTTPS servers and sites rather than HTTP
	ones for higher security.
