module.exports = {
  title: "pimatic-obi-power-outlet device config schemas"
  ObiPowerOutlet:
    title: "OBI Power Outlet config"
    type: "object"
    properties: {
      ipAddress:
        description: "IP address of the power outlet"
        type: "string"
    }
}
