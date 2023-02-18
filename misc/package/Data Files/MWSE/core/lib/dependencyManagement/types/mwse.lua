return {
    id = "mwse",
    checkDependency = function(self, dependency)
        if not dependency.buildnumber then return true end
        self.logger:debug("Checking MWSE version: %s", dependency.buildnumber)
        self.logger:debug("Current MWSE version: %s", mwse.buildNumber)
        if dependency.buildnumber > mwse.buildNumber then---@diagnostic disable-line: undefined-field
            self.logger:error("MWSE is out of date")
            return false, {
                {
                    title = "MWSE Update Required",
                    reasons = {
                        string.format("Current Build: %d; Required: %s",
                            mwse.buildNumber, dependency.buildnumber)---@diagnostic disable-line: undefined-field
                    },
                    resolveButton = {
                        text = "Update MWSE",
                        tooltip = "Run 'Morrowind/MWSE-Update.exe'",
                        callback = function()
                            local updateExe = "start .\\MWSE-Update.exe"
                            os.execute(updateExe)
                            os.exit()
                        end
                    }
                }
            }
        end
        return true
    end
}