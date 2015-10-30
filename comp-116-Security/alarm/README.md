Louis Ades
alarm.rb

1. The web server log analysis has been implemented successfully.
   Syntax seems completely correct on the live stream analysis, but I
   honestly do not know how to properly test it aside from accessing random
   websites and ensuring there are no syntax errors in the code.
   UPDATE: Credit card data being leaked has been tested properly--it works.
   There is no code change in this next update--it's just one more thing I
   tested.

2. I collaborated for about an hour with Jeremy Goldman on 10/7/14.

3. I probably spent 3 hours learning Ruby, PacketFu and troubleshooting some
   Kali problems I had with installing PacketFu. I spent probably another 5
   or 6 total in coding, looking at documentation (the documentation for 
   PacketFu NEEDS work), troubleshooting, and debugging until the final 
   product was ready. Another hour or so thinking about and writing the README
   and the answers to questions.

Answers to Questions:
1. Hell no, they're not that good! If it was this easy to have a be-all end-all
   anti-scanning/leaking/shellcode alarm system, and if it only took <100 lines
   of code, there wouldn't be full software packages costing hundreds of
   dollars ensuring better detection and protection! There are several flaws
   and gaping holes here and there--packets can be tampered with  so that 
   it's much more difficult to parse them as easily as we did. Plaintext credit
   cards can be leaked in other ways than our simple parsing had--it could just
   be really bad encryption, for example, where credit cards won't show up as
   leaked in our alarm but in reality they are still ridiculously easy to crack.
   Scanning can be done in other ways than NULL scan and XMAS scan, and there
   are probably ways to disguise them and the flags as if they were coming from
   a random, safe website. These are just a few ways I can think of where the
   alarm can miss stuff or where a clever hacker can outsmart it.

2. I'd probably give it more functionality to detect a wider range of scans.
   I'd also give it functionality to detect when *I* have post requests where
   I have private data leaking to unsafe sites unintentionally. It would be
   phenomenal if incidents like that can be caught in the act. If I can see
   where my computer is sending my private information, it'd be another way
   to catch the cracker in the act.