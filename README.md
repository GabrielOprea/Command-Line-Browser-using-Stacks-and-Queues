# Command-Line-Browser-using-Stacks-and-Queues
Program that implements a Browser that can download resources and share some of the functionalities of a regular Web Browser, implemented using Queues, Stacks, Linked Lists, Priority Queues.

For the implementation of this project I used the following functions:
The initTab() function that initializes a tab for my browser.
The insertTab() function that adds a tab to the existing one called browser.
The adrAccess() function opens a web page in the current tab, if there is already a web page in that tab it puts it in the back stack.
The deleteTab() function deletes the tab from the end of the list and restores the links, also frees the memory from the back and forward stacks and web page in the browser.
The print() function displays for each tab a required format if has or does not have a web page inside.
The tabChange() function modifies the current tab so that it is the one indicated by the index.
The antPage() function takes its web page from the current tab if it exists and puts it in the forward stack, then removes the new web page of the current tab from the back stack.
The nextPage() function takes its web page from the current tab if it exists and it sets it in the back stack, then remove the new web page of the current tab from the forward stack.
The printHistory() function retrieves the urls it displays from the history queue at the output and also use an auxiliary queue so as not to lose history.
The del_G_history() function deletes the first or most accessed nr_entries(number of entries) from the global history if nr_entries is 0 or a number greater than the number of total history entries.
The listDl() function displays at the output the resources on the current page that can be downloaded.
The downloadResource() function adds a resource to the function's downloads queue of dimension remaining to download the resource.
The wait() function has a case where the download of a resource could not be completed completely in which case I put it in the backUp queue.Other situation can also be the one when the download was done completely and the resource was put in the finished queue. After this checks I concatenated the backUp queue on finished one.
The printDownloads() function displays the download history in the browser.
The destroyBrowser() function frees the memory allocated for tabs as well as browser (= initial tab).
The stay() function returns the remaining size for download from a resource.
The destroyPageStack() function frees memory for a stack of web pages.
The destroyHistory() function frees the memory for the queue in which we have kept the global history.
In int main() function I initialized the queue representing the browser history as well as the one intended for downloads and I declared the browser variable that represents the tab initially to which I added the following tabs depending on the command stored in cmd, otherwise I said I used the browser tab as a sentry.
As for the stack and queue functions, I modified them using the simply chained generic lists.
