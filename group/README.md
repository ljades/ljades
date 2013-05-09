README
======

Title
-----
Joey++

Problem Statement 
------------------

The Joey route is inefficient. It goes all the way over to Teale Square just to return to the Campus Center and we believe that as a community, we can design a better Joey route. One that for example, manages the tradeoff of not stopping at the Campus Center twice in exchange for faster turnaround. Of course there are tradeoffs but as a community, we might be able to improve a system that we all depend on to better suit our needs.

Problem Solver
---------------
We allow users to interactively manipulate the Joey route and then we calculate runtimes, generate timetables and then present such data along with other pros and cons to the users for interpretation.

Features We Will Implement
--------------------------
1. Route Changer
1. Add/Remove Stops
1. Share your route via social media.
1. Time, Distance Calculator
1. Geolocation
1. Bootstrap
1. Reporting Charts and Graphs
1. Data Storage via AWS
1. Emails via SendGrid

Data Collection
---------------
The only data we collect will be the user input of routes and perhaps other related opinions.

Algorithms
----------
Distance
Computing trip times

#Comments by Ming
* Good

Things we still need to do:
* Fix the dragging/route disappearing issue. 
* Figure out a way to extract the route from the map, rather than need to simply create a new one manually. We need this so that when he hit the save button, we can post the updated route to the database.
* Fix the New Stop button.  (DONE!!! Silly map passing...)
* Give routes IDs. They could be strings like names, for all we care.
* (I don't know what database stuff is missing. It would be Brendan who's overseeing that stuff.)
* Once the above is done, make the normal Joey route a default route of some sort, but implement a GET request that, if successful, will receive the route that we need instead. If request fails, use default Joey route.
* To implement requesting routes, we'll need to include a button on the main page that when clicked will open a form or text box, and then the input from this will be used to find a route. If it can't find one under that username/name of route/ID (whatever we end up doing), it keeps up the default Joey route.
* Lastly, we need to make CSS refinements if need be.