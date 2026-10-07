local Rock = {}

function Rock:OnCreate()
    self.Bounces = 0
    self.BounceText = engine.FindEntity("BounceText")
end

-- Called by the physics system when a solid collider starts touching this one.
function Rock:OnCollisionEnter(other)
    self.Bounces = self.Bounces + 1

    if self.BounceText and self.BounceText:IsValid() then
        self.BounceText:GetComponent(TextComponent).Text = "Bounces: " .. self.Bounces
    end

    -- Returns the existing request if one is already pending, so sounds don't stack.
    self:AddComponent(PlaySoundRequestComponent)

    print("Rock hit " .. other:GetName())
end

return Rock