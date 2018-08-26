module.exports = (env) ->
  Promise = env.require 'bluebird'
  child_process = require("child_process")

  class ObiPowerOutlet extends env.plugins.Plugin
    init: (app, @framework, @config) =>
      deviceConfigDef = require("./device-config-schema")

      statusQueryInterval = @config.statusQueryInterval

      @framework.deviceManager.registerDeviceClass("ObiPowerOutlet", {
        configDef: deviceConfigDef.ObiPowerOutlet, 
        createCallback: (config) => new ObiPowerOutlet(config)
      })

      class ObiPowerOutlet extends env.devices.PowerSwitch
        actions: 
          turnOn:
            description: "turns the switch on"
          turnOff:
            description: "turns the switch off"

        turnOn: -> @changeStateTo true
        turnOff: -> @changeStateTo false

        constructor: (@config) ->
          @name = @config.name
          @id = @config.id
          @ipAddress = @config.ipAddress

          updateState = () ->
            child_process.exec("curl -m 3 -s http://" + @ipAddress + "/status", ((err, stdout, stderr) ->
              @_setState(stdout == "on\n")
            ).bind(this))
          updateState.bind(this)()
          @statusInterval = setInterval updateState.bind(this), statusQueryInterval

          super()

        changeStateTo: (state) ->
          if @_state is state then return Promise.resolve()
          action = "off"
          if state then action = "on"
          child_process.exec("curl -m 3 -s http://" + @ipAddress + "/" + action, ((err, stdout, stderr) ->
            @_setState(stdout == "on\n")
          ).bind(this))
          return Promise.resolve()

        getState: () ->
          return @_state

        destroy: () ->
          clearInterval(@statusInterval) if(@statusInterval)
          super()

  obiPowerOutlet = new ObiPowerOutlet
  return obiPowerOutlet
