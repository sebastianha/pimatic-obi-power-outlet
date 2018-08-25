module.exports = {
  title: "obi power outlet config options"
  type: "object"
  properties:
    statusQueryInterval:
      description: "Interval for querying the status of the devices"
      type: "number"
      default: 10000
}
