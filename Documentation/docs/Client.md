The client component of this project consists of a frontend created using React JS, allowing users a comfortable interface to interact with their data.

The "client" directory of the project, containing the source code, consists of three main directories: "components", "pages", and "contexts"

## Components

This directory contains all components used throughout the project. This includes things like the Navigation Bar or "NavBar", the User Login card or "UserLogin", the Stock Card used to show a single company's stock information or "StockCard" etc. These all are in folders of the same name, along with a "styles.css" file for styling.

---
## Contexts

This directory contains all the contexts provided to pages throughout the project. These are only two: an authorization context, or "AuthContext"; and a server context, or "ServerContext". These give the pages a means to understand authorization and server contexts of a page, by means of parsing the localstorage and looking for a token for authorization; or connecting to the server by means of a websocket and sharing the details of that.

---
## Pages

This directory contains all the pages viewed to the user. These include the home page, the stocks page, the wallet page, and the transactions page. These all use the components and contexts provided to appropriately render details to the user.
